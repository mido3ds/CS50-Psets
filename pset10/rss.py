# -*- coding: utf-8 -*-
import bs4,io

with io.open('rssex.html', encoding='utf-8') as f:
    rssexFile = bs4.BeautifulSoup(f.read(), 'html.parser')

items = rssexFile.rss.channel.find_all('item')

with io.open('rss2.html', 'w', encoding='utf-8') as f:
    f.write(str(items))