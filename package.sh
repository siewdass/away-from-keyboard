#!/usr/bin/env bash
cp afk package/opt/afk/ > /dev/null 2>&1
dpkg-deb -Z xz -b package afk.deb > /dev/null 2>&1
rm package/opt/afk/afk > /dev/null 2>&1
