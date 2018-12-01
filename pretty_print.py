#!/usr/bin/python3

import json

f = open("temp.txt", "r");
j = json.load(f);

print("%5s %15s %30s %6s %6s %6s" % ("rank", "name", "path", "games", "mu", "sigma"))
items = j['items'];
ranks = [items[i]['rank'] for i in range(0, len(items))]
sorted_items = [(items[i]['rank'], items[i]) for i in range(0, len(items))]
sorted_items.sort();
for s_item in sorted_items:
    item = s_item[1]
    print("%5d %15s %30s %6d %6.2f %6.2f" % (item['rank'], item['name'], item['path'], item['games_played'], item['mu'], item['sigma']))
