.PHONY: build run shell clean

# Build the docker image
build:
	docker-compose build

# Run the container and execute the script
run:
	docker-compose run link_experiment bash -c "./link_syscalls.sh"

# Open interactive shell into container
shell:
	docker-compose run link_experiment bash

# Cleanup containers and images
clean:
	docker-compose down --rmi all --volumes --remove-orphans
