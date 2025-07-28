winman: windowmanager.h windowmanager.c hyprland.o
	gcc -c windowmanager.c

hyprland.o: hyprland.h hyprland.c
	gcc -c `pkg-config --cflags json-c` hyprland.c

setupTest: test/setupTest.c winman
	gcc `pkg-config --libs json-c` windowmanager.o hyprland.o test/setupTest.c -o test/setupTest

runSetupTest: setupTest
	./test/setupTest

hyprDispatchWorkspace: test/hyprDispatchWorkspace.c winman
	gcc `pkg-config --libs json-c` windowmanager.o hyprland.o test/hyprDispatchWorkspace.c -o test/hyprDispatchWorkspace

runHyprDispatchWorkspace: hyprDispatchWorkspace
	./test/hyprDispatchWorkspace