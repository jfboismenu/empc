FROM centos
RUN yum -y install bzip2 which python3
RUN mkdir /tmp/docker
COPY docker/* /tmp/docker/
RUN mkdir -p /opt
RUN tar xjvf /tmp/docker/sourceryg++-2016.11-64-ia16-elf-x86_64-linux-gnu.tar.bz2 -C /opt
RUN python3 /tmp/docker/symlink-sourcery.py