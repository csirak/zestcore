# Default variables
TEMPLATE_DIR := template
NAME ?= undefined

.PHONY: all clean module

all:
	@echo "Please specify a target"

clean:
	rm -rf *.vcd *.log

# Create new module from template
module:
ifeq ($(NAME),undefined)
	@echo "Usage: make module NAME=<name>"
	@exit 1
endif
	@if [ -d "../$(NAME)" ]; then \
		echo "Error: Module directory '../$(NAME)' already exists"; \
		exit 1; \
	fi
	@echo "Creating new module: $(NAME)"
	@cp -r $(TEMPLATE_DIR) $(NAME)
	@sed -i '' 's/NAME/$(NAME)/g' ./$(NAME)/Makefile
	@sed -i '' 's/NAME/$(NAME)/g' ./$(NAME)/tb.cpp
	@sed -i '' 's/NAME/$(NAME)/g' ./$(NAME)/compile_commands.json
	@echo "\`timescale 1ns/1ps\n\nmodule $(NAME) (\n\n);\n\nendmodule" > ./$(NAME)/$(NAME).sv
	@echo "Module $(NAME) created successfully"

