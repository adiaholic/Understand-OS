FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y strace coreutils bash nano && \
    apt clean

WORKDIR /experiment

COPY link_syscalls.sh .

RUN chmod +x link_syscalls.sh

CMD ["bash"]
