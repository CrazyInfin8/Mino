package main

import (
	"encoding/xml"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func unwrap[T any](value T, err error) T {
	if err != nil {
		panic(err)
	}
	return value
}

func must(err error) {
	if err != nil {
		panic(err)
	}
}

type SVG struct {
	Path []Path `xml:"path"`
}

type Path struct {
	Shape string `xml:"d,attr"`
	Style string `xml:"style,attr"`
}

func varName(path string) string {
	return strings.NewReplacer("/", "_", "-", "_", ".", "__").Replace(path)
}
func main() {
	output := os.Stdout
	output.WriteString(
		`
typedef struct Op {
	enum {
		Op_Move,
		Op_Line,
		Op_Cube,
		Op_Quad,
		Op_Arc,
		Op_Close,

	} type;
	float points[6];
} Op;

typedef struct Path {
	int len;
	Op *op;
} Path;

typedef struct Shape {
	int len;
	Path *path;
} Shape;

`,
	)
	for _, arg := range os.Args[1:] {
		var svg SVG

		must(xml.Unmarshal(unwrap(os.ReadFile(arg)), &svg))

		parsePath(svg, varName(arg))

	}
}

func parsePath(svg SVG, name string) {
	fmt.Printf("const Shape %s = {\n\t.len = %d,\n\t.path = (Path[]){\n", name, len(svg.Path))
	for _, path := range svg.Path {
		fmt.Printf("\t\t{\n\t\t\t.op = (Op[]){\n")
		reader := strings.NewReader(path.Shape)
		var opCount = 0
		for {
			var (
				c   byte
				err error
			)
			c, err = reader.ReadByte()
			for c == ' ' && err == nil {
				c, err = reader.ReadByte()
			}
			if err != nil {
				break
			}
			switch c {
			case 'M':
				f := unwrap(parseNumbers(reader, 2))
				fmt.Printf("\t\t\t\t{.type = Op_Move, .points = { %f, %f }},\n", f[0], f[1])
				opCount++
			case 'L':
				f := unwrap(parseNumbers(reader, 2))
				fmt.Printf("\t\t\t\t{.type = Op_Line, .points = { %f, %f }},\n", f[0], f[1])
				opCount++
			case 'Z':
				fmt.Printf("\t\t\t\t{.type = Op_Close},\n")
				opCount++
			case 'C':
				f := unwrap(parseNumbers(reader, 6))
				fmt.Printf("\t\t\t\t{.type = Op_Cube, .points = { %f, %f, %f, %f, %f, %f }},\n", f[0], f[1], f[2], f[3], f[4], f[5])
				opCount++
			case 'Q':
				f := unwrap(parseNumbers(reader, 4))
				fmt.Printf("\t\t\t\t{.type = Op_Quad, .points = { %f, %f, %f, %f}},\n", f[0], f[1], f[2], f[3])
				opCount++
			}
		}
		fmt.Printf("\t\t\t},\n\t\t\t.len = %d,\n\t\t},\n", opCount)
	}
	fmt.Printf("\t},\n};\n")
}

func parseNumbers(reader *strings.Reader, count int) (nums []float32, err error) {
    nums = make([]float32, count)
    for i := 0; i < count; i++ {
        var numStr string
        for {
            c, err := reader.ReadByte()
            if err != nil {
                return nil, err
            }
            if c >= '0' && c <= '9' || c == '.' || c == '-' {
                numStr += string(c)
            } else if len(numStr) > 0 {
                break
            }
        }
        num, err := strconv.ParseFloat(numStr, 32)
        if err != nil {
            return nil, err
        }
        nums[i] = float32(num)
    }
    return nums, nil
}