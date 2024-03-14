FROM ubuntu:18.04

ARG USER_NAME="foam"
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN apt update && apt upgrade -y

RUN apt install sudo build-essential cmake make git gcc python3 vim -y

RUN apt install mpi-default-dev flex libscotch-dev -y

# add non-root user
RUN groupadd --gid ${USER_GID} ${USER_NAME} && \
  useradd --uid ${USER_UID} --gid ${USER_GID} -s /bin/bash -m ${USER_NAME} && \
  echo "${USER_NAME} ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/${USER_NAME} && \
  chmod 0440 /etc/sudoers.d/${USER_NAME}

COPY . /home/${USER_NAME}/porefoam
RUN chown -R ${USER_NAME}:${USER_NAME} /home/${USER_NAME}/porefoam

USER ${USER_NAME}
WORKDIR /home/${USER_NAME}/porefoam

RUN make -j

RUN make test

RUN make clean

RUN echo "source ~/porefoam/src/script/bashrc" >> ~/.bashrc