package main

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"regexp"
	"strings"
)

var (
	inputPaths      []string
	outputPath      string
	defines         map[string]string = make(map[string]string)
	includeDirs     map[string]string = make(map[string]string)
	includedFiles   map[string][]byte = make(map[string][]byte)
	// directivesStack []string
	// ignoreLevel     int = -1
)

func must(err error) {
	if err != nil {
		panic(err)
	}
}

func unwrap[T any](value T, err error) T {
	if err != nil {
		panic(err)
	}
	return value
}

func main() {
	must(configure())

	outputFile := os.Stdout
	if outputPath != "" {
		outputFile = unwrap(os.Create(outputPath))
	}

	for _, path := range inputPaths {
		path = sanitizePath(path)

		if _, ok := includedFiles[path]; ok {
			continue
		}

		includedFiles[path] = unwrap(os.ReadFile(path))
		must(writeFile(path, includedFiles[path], outputFile))
	}

	for define, value := range defines {
		fmt.Printf("	\"%s\": \"%s\"\n", define, value)
	}
}

func sanitizePath(path string) string {
	return unwrap(filepath.Abs(path))
}

func configure() error {
	for _, arg := range os.Args[1:] {
		switch {
		case strings.HasPrefix(arg, "-o"):
			outputPath = arg[2:]
		case strings.HasPrefix(arg, "-D"):
			setter := strings.IndexByte(arg, '=')
			if setter == -1 {
				defines[arg[2:]] = "1"
			} else {
				defines[arg[2:setter]] = arg[setter+1:]
			}
		case strings.HasPrefix(arg, "-I"):
			path := sanitizePath(arg[2:])
			dir := unwrap(os.Open(path))
			if !unwrap(dir.Stat()).IsDir() {
				return errors.New("include path is not a directory")
			}
			includeDirs[path] = path
		default:
			inputPaths = append(inputPaths, arg)
		}
	}
	if len(inputPaths) == 0 {
		return errors.New("No input files specified")
	}
	return nil
}

var (
	// ifExp      = regexp.MustCompile(`^\s*#\s*if\s*(.*?)\s*(?:\/\/.*)?$`)
	// elifExp    = regexp.MustCompile(`^\s*#\s*elif\s*(.*?)\s*(?:\/\/.*)?$`)
	// endifExp   = regexp.MustCompile(`^\s*#\s*endif`)
	// ifdefExp   = regexp.MustCompile(`^\s*#\s*ifdef\s*(\w+)`)
	// ifndefExp  = regexp.MustCompile(`^\s*#\s*ifndef\s*(\w+)`)
	// defineExp  = regexp.MustCompile(`^\s*#\s*define\s*(\w+)\s*(.*?)(?:\s*\/\/.*)?$`)
	// undefExp   = regexp.MustCompile(`^\s*#\s*undef\s*(\w+)\s*(?:\s*\/\/.*)?$`)
	includeExp = regexp.MustCompile(`^\s*#\s*include\s*"([\w\.\/\\]+)"`)
)

func findInclude(origin, path string) (read bool, fullPath string) {
	fullPath = sanitizePath(path)
	if _, ok := includedFiles[fullPath]; ok {
		return true, fullPath
	}

	src, err := os.ReadFile(fullPath)
	if err == nil {
		includedFiles[fullPath] = src
		return false, fullPath
	}

	for _, dir := range includeDirs {
		fullPath = filepath.Join(dir, path)
		if _, ok := includedFiles[fullPath]; ok {
			return true, fullPath
		}

		src, err := os.ReadFile(fullPath)
		if err == nil {
			includedFiles[fullPath] = src
			return false, fullPath
		}
	}
	return false, ""
}

func writeFile(inPath string, src []byte, out *os.File) error {
	in := bytes.NewBuffer(src)
	var (
		line string
		err  error
	)
	for {
		line, err = in.ReadString('\n')
		if match := includeExp.FindStringSubmatch(line); match != nil {
			read, path := findInclude(inPath, match[1])
			if path == "" {
				return errors.New("Could not find path: " + path)
			}

			if read {
				continue
			}

			err := writeFile(path, includedFiles[path], out)
			if err != nil {
				return err
			}

			continue
		}

		out.WriteString(line)

		if err != nil {
			if err == io.EOF {
				out.WriteString("\n")
				return nil
			}
			return err
		}
	}

}
