#!/bin/bash

cat Version.h | sed 's/DRAGON/DRAGON_RECIPES/g' > DragonRecipes/include/DragonRecipes/Version.h
cat Version.h | sed 's/DRAGON/CASTLE/g' > Castle/Version.h
cat Version.h | sed 's/DRAGON/KNIGHT/g' > Knight/include/Version.h
