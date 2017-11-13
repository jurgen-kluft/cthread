package main

import (
	"github.com/jurgen-kluft/xcode"
	"github.com/jurgen-kluft/xthread/package"
)

func main() {
	xcode.Generate(xthread.GetPackage())
}
