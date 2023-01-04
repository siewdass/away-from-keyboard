#!/usr/bin/env bash
cp afk package/opt/afk/
dpkg-deb -Z xz -b package afk.deb
rm package/opt/afk/afk
