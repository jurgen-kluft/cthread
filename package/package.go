package xthread

import (
	"github.com/jurgen-kluft/xbase/package"
	"github.com/jurgen-kluft/xcode/denv"
	"github.com/jurgen-kluft/xentry/package"
	"github.com/jurgen-kluft/xunittest/package"
)

// GetPackage returns the package object of 'xthread'
func GetPackage() *denv.Package {
	// Dependencies
	xunittestpkg := xunittest.GetPackage()
	xentrypkg := xentry.GetPackage()
	xbasepkg := xbase.GetPackage()

	// The main (xthread) package
	mainpkg := denv.NewPackage("xthread")
	mainpkg.AddPackage(xunittestpkg)
	mainpkg.AddPackage(xentrypkg)
	mainpkg.AddPackage(xbasepkg)

	// 'xthread' library
	mainlib := denv.SetupDefaultCppLibProject("xthread", "github.com\\jurgen-kluft\\xthread")
	mainlib.Dependencies = append(mainlib.Dependencies, xbasepkg.GetMainLib())

	// 'xthread' unittest project
	maintest := denv.SetupDefaultCppTestProject("xthread_test", "github.com\\jurgen-kluft\\xthread")
	maintest.Dependencies = append(maintest.Dependencies, xunittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xentrypkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xbasepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
