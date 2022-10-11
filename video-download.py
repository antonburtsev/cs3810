#!/usr/bin/env python3

# Updated download scripts, run the two commands in the folder in which you have index.html
# grep -Eo "(http|https)://[a-zA-Z0-9./?=_%:-]*" index.html | grep 'utah.zoom.us' > urls.txt
# grep -Eo "Lecture[a-zA-Z0-9./?=_%:-\ \ -\ ]*" index.html > titles.txt
#
# and then 
#
# python3 video-download.py
#
#

import multiprocessing
import os
import shlex

urls = open('urls.txt', 'r')
titles = open('titles.txt', 'r')

pool = multiprocessing.Pool(10)
commands = []

for (url, title) in zip(urls, titles):
    url = url.strip()
    title = title.strip()
    filename = "{}.mp4".format(title)

    print("Downloading {}".format(title))

    os.system("yt-dlp -o {} {}".format(
        shlex.quote(filename),
        shlex.quote(url),
    ))
