Main:Main.c UI.c Game.c
	gcc $^ -o $@

.PHONY:clean
		rm -i Main.c UI.c UI.h Game.h Game.c Snake.c
