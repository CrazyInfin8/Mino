package main

import (
	"bytes"
	"errors"
	"fmt"
	"os"
	"strconv"
	"strings"

	"github.com/andybalholm/brotli"
)

var (
	inputPaths []string
	outputPath string
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

// type CFileWriter struct {
// 	out io.Writer;
// }

// func (writer *CFileWriter) Write(p []byte) (n int, err error) {
// 	writer.Write()
// }

func pathToVarName(path string) string {
	return strings.NewReplacer("/", "_", "-", "_", ".", "__").Replace(path)
}

func main() {
	must(configure())

	outputFile := os.Stdout
	if outputPath != "" {
		outputFile = unwrap(os.Create(outputPath))
	}

	outputFile.WriteString(
		`typedef struct Asset {
	const int decodedSize, size;
	const unsigned char* const data;
} Asset;

`)

	for _, path := range inputPaths {

		data := unwrap(os.ReadFile(path))

		buf := bytes.Buffer{}
		encode := brotli.NewWriterOptions(&buf, brotli.WriterOptions{
			Quality: brotli.BestCompression,
			LGWin:   0,
		})
		unwrap(encode.Write(data))
		encode.Close()

		writeAsset(outputFile, pathToVarName(path), len(data), buf.Bytes())

		// fmt.Printf("Data size: %d, compressed size: %d\n", len(data), len(buf.Bytes()))
	}
}

func configure() error {
	for _, arg := range os.Args[1:] {
		switch {
		case strings.HasPrefix(arg, "-o"):
			outputPath = arg[2:]
		default:
			inputPaths = append(inputPaths, arg)
		}
	}
	if len(inputPaths) == 0 {
		return errors.New("No input files specified")
	}
	return nil
}

func writeAsset(file *os.File, varName string, oldLen int, data []byte) {
	file.WriteString("const Asset ")
	file.WriteString(varName)
	file.WriteString(" = (Asset) {\n\t.decodedSize = ")
	file.WriteString(strconv.Itoa(oldLen))
	file.WriteString(",\n\t.size = ")
	file.WriteString(strconv.Itoa(len(data)))
	file.WriteString(",\n\t.data = (unsigned char[")
	file.WriteString(strconv.Itoa(len(data)))
	file.WriteString("]){")
	for i, c := range data {
		if i%64 == 0 {
			file.WriteString("\n\t\t")
		}
		fmt.Fprintf(file, "0x%02X,", c)
	}
	file.WriteString("\n\t},\n};\n\n")
}
