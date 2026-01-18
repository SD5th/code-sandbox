docker-build:
	docker build -t ant-movement .

docker-run:
	docker run --rm ant-movement

docker-build-and-run:
	docker build -t ant-movement . && docker run --rm ant-movement

docker-build-debug:
	docker build -t ant-movement-debug .

docker-run-debug:
	docker run --rm -it --entrypoint sh ant-movement-debug