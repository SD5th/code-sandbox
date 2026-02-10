.PHONY: build, run, build-and-run, stop, shell

build:
	cd docker && docker-compose build

build-no-buildkit:
	cd docker && DOCKER_BUILDKIT=0 docker-compose build

run:
	cd docker && docker-compose up

build-and-run:
	cd docker && docker-compose up --build

stop:
	cd docker && docker-compose down

shell:
	cd docker && docker-compose exec cellular-automaton sh