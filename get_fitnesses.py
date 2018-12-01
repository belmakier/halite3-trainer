import subprocess
import json

f = open("temp.txt", "r");
out = open("fitnesses.txt", "w");
j = json.load(f);

for item in j['items']:
    if (item['name']=="bot_v4" or
        item['name']=="bot_v5" or
        item['name']=="bot_v6" or
        item['name'][:8]=="bot_v6.2"):
        continue
    print(item['name'])               
    out.write("%6.3f   %d\n" %(item['mu']-3*item['sigma'], int(item['name'][7:])))
    

f.close()
out.close()
