FROM ubuntu:14.04

RUN apt-get update && apt-get install build-essential -y \
    libssl-dev \
    libdb++-dev \
    libboost-all-dev \
    libqrencode-dev \
    wget

ENV sarDir /sar

RUN mkdir sarDir
WORKDIR ${sarDir}

ADD http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.6.tar.gz ./miniupnpc-1.6.tar.gz
RUN tar -xvf miniupnpc-1.6.tar.gz
RUN cd miniupnpc-1.6 && make && make install

ADD ./share ./share
ADD ./src ./src
RUN cd src/ && make -f makefile.unix

RUN mkdir -p /root/.SARCoin
RUN wget -O /root/.SARCoin/bootstrap.dat https://github.com/sarcoin/SARCoin/releases/download/1.5.0/bootstrap.dat
ADD ./conf/* /root/.SARCoin/
RUN chmod 400 /root/.SARCoin/SARCoin.conf

CMD ["./src/SARCoind", "-logtimestamps"]
