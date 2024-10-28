.DEFAULT_GOAL := help

# AutoDoc
# -------------------------------------------------------------------------
.PHONY: help
help: ## This help
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)
.DEFAULT_GOAL := help

.PHONY: release
release: ## Generate release build
	cmake --preset release
	cmake --build build

.PHONY: debug
debug: ## Generate debug build
	cmake --preset debug
	cmake --build build

.PHONY: clear
clear: ## Clear build directory
	rm -rf build
