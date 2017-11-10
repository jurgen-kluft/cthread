package xrandom

import (
	"github.com/jurgen-kluft/xbase/package"
	"github.com/jurgen-kluft/xcode/denv"
	"github.com/jurgen-kluft/xentry/package"
	"github.com/jurgen-kluft/xunittest/package"
)

// GetPackage returns the package object of 'xrandom'
func GetPackage() *denv.Package {
	// Dependencies
	xunittestpkg := xunittest.GetPackage()
	xentrypkg := xentry.GetPackage()
	xbasepkg := xbase.GetPackage()

	// The main (xrandom) package
	mainpkg := denv.NewPackage("xrandom")
	mainpkg.AddPackage(xunittestpkg)
	mainpkg.AddPackage(xentrypkg)
	mainpkg.AddPackage(xbasepkg)

	// 'xrandom' library
	mainlib := denv.SetupDefaultCppLibProject("xrandom", "github.com\\jurgen-kluft\\xrandom")
	mainlib.Dependencies = append(mainlib.Dependencies, xbasepkg.GetMainLib())

	// 'xrandom' unittest project
	maintest := denv.SetupDefaultCppTestProject("xrandom_test", "github.com\\jurgen-kluft\\xrandom")
	maintest.Dependencies = append(maintest.Dependencies, xunittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xentrypkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xbasepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
