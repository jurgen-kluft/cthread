package main

import (
	"github.com/jurgen-kluft/xcode"
	"github.com/jurgen-kluft/xrandom/package"
)

func main() {
	xcode.Generate(xrandom.GetPackage())
}
