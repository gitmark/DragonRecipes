#!/bin/bash

astyle  --style=google  ./*.cpp,*.h
astyle  --style=google  --recursive  DragonRecipes/*.cpp,*.h Castle/*.cpp,*.h  Knight/*.cpp,*.h


