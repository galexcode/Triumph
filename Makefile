default:
	@echo "This is the root makefile for Triumph"
	@echo ""
	@echo "Use one of the following commands:"
#	@echo "---------------------------------------------------------------------"
#	@echo " $(MAKE) win32-mingw    to compile for Windows using MinGW"
#	@echo " $(MAKE) mingw-clean    to remove any files generated for this target"
	@echo "---------------------------------------------------------------------"
	@echo " $(MAKE) x11            to compile for X11 on Unix-like systems"
	@echo " $(MAKE) x11-clean      to remove any files generated for this target"
	@echo "---------------------------------------------------------------------"
#	@echo " $(MAKE) cocoa          to compile for Mac OS X"
	@echo " $(MAKE) cocoa-clean    to remove any files generated for this target"
	@echo "---------------------------------------------------------------------"
	@echo " There are also Microsoft Visual C++ 2010 and XCode 4 project files  "
	@echo " available in this directory"
	@echo "---------------------------------------------------------------------"

#################################################################################
# X11 on Unix-like Systems
#################################################################################

MAKEFILE_X11    = Triumph/Makefile.x11
MAKEFILE_OSX    = Triumph/Makefile.osx

x11-clean: $(MAKEFILE_X11)
	cd Triumph && $(MAKE) -f Makefile.x11 clean

x11: $(MAKEFILE_X11)
	cd Triumph && $(MAKE) -f Makefile.x11
    
cocoa-clean: $(MAKEFILE_OSX)
	cd Triumph && $(MAKE) -f Makefile.osx clean    
    
    
    
