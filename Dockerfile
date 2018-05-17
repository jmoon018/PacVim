# Build
FROM alpine:3.7
RUN apk --no-cache add \
    make \
    g++ \
    ncurses-dev \
    ncurses-static \
    binutils
RUN ln -s /usr/include/ncurses.h /usr/include/cursesw.h \
 && ln -s /usr/lib/libncursesw.a /usr/lib/libncurses.a
WORKDIR /root/pacvim
COPY . .
ENV CXXFLAGS='-Os -flto -ffunction-sections -fdata-sections'
ENV LDFLAGS='-static -s -Wl,--gc-sections'
RUN make -j4 install
RUN strip -R.comment -R.note /usr/local/bin/pacvim

# Run
FROM alpine:3.7
RUN apk --no-cache add \
    ncurses-terminfo-base \
 && adduser -Du1000 pacvim
USER pacvim
WORKDIR /home/pacvim
COPY --from=0 /usr/local/bin/pacvim /usr/local/bin/pacvim
COPY --from=0 /usr/local/share/pacvim-maps /usr/local/share/pacvim-maps
ENTRYPOINT ["/usr/local/bin/pacvim"]
