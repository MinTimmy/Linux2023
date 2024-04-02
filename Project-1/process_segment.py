# segments = ["text", 'data', 'bss', "heap", "mm", 'stack']
segments = ['stack',"mm","heap",'bss', 'data',  "text",  ]
file_path = "process_segment.txt"
result = dict()

def new_process(pid):
    r = dict()
    for s in segments:
        r[s] = {
            "vma_start" : {
                "start" : -1,
                "end"   : -1
            },
            "vma_end"   : {
                "start" : -1,
                "end"   : -1
            }
        }
    result[pid] = r
curr_pid = -1
with open(file_path, "r") as file:
    for line in file:
        # Get the pid
        if len(line) < 10:
            new_process(line[:-1])
            curr_pid = line[:-1]
            continue
        
        data = line.split(',')
        result[curr_pid][data[0]]["vma_start"]["start"] = data[1]
        result[curr_pid][data[0]]["vma_start"]["end"] = data[2]
        result[curr_pid][data[0]]["vma_end"]["start"] = data[3]
        result[curr_pid][data[0]]["vma_end"]["end"] = data[4]

pids = result.keys()
pids = list(pids)
# # print(list(pids)[0])
# for p in pids:
#     print(p, end = ' ')
#     for s in result[pids[0]]:
#         print(s, end=',')
#         for vma in result[pids[0]][s]:
#             for se in result[pids[0]][s][vma]:
#                 print(result[p][s][vma][se], end =' ')
#             print(end=',')
#     print()
# print(result)
        

result_csv = f"""
pid, {pids[0]}  , {pids[1]}, {pids[2]}
stack, {result[pids[0]]['stack']["vma_start"]["start"]} {result[pids[0]]['stack']["vma_end"]["start"]}, {result[pids[1]]['stack']["vma_start"]["start"]} {result[pids[1]]['stack']["vma_end"]["start"]}, {result[pids[2]]['stack']["vma_start"]["start"]} {result[pids[2]]['stack']["vma_end"]["start"]}
stack, {result[pids[0]]['stack']["vma_start"]["end"]} {result[pids[0]]['stack']["vma_end"]["start"]}, {result[pids[1]]['stack']["vma_start"]["start"]} {result[pids[1]]['stack']["vma_end"]["start"]}, {result[pids[2]]['stack']["vma_start"]["start"]} {result[pids[2]]['stack']["vma_end"]["start"]}
mm,{result[pids[0]]['mm']["vma_start"]["start"]} {result[pids[0]]['mm']["vma_end"]["start"]}, {result[pids[1]]['mm']["vma_start"]["start"]} {result[pids[1]]['mm']["vma_end"]["start"]}, {result[pids[2]]['mm']["vma_start"]["start"]} {result[pids[2]]['mm']["vma_end"]["start"]}
mm,{result[pids[0]]['mm']["vma_start"]["end"]} {result[pids[0]]['mm']["vma_end"]["start"]}, {result[pids[1]]['mm']["vma_start"]["start"]} {result[pids[1]]['mm']["vma_end"]["start"]}, {result[pids[2]]['mm']["vma_start"]["start"]} {result[pids[2]]['mm']["vma_end"]["start"]}
heap, {result[pids[0]]['heap']["vma_start"]["start"]} {result[pids[0]]['heap']["vma_end"]["start"]}, {result[pids[1]]['heap']["vma_start"]["start"]} {result[pids[1]]['heap']["vma_end"]["start"]}, {result[pids[2]]['heap']["vma_start"]["start"]} {result[pids[2]]['heap']["vma_end"]["start"]}
heap, {result[pids[0]]['heap']["vma_start"]["end"]} {result[pids[0]]['heap']["vma_end"]["start"]}, {result[pids[1]]['heap']["vma_start"]["start"]} {result[pids[1]]['heap']["vma_end"]["start"]}, {result[pids[2]]['heap']["vma_start"]["start"]} {result[pids[2]]['heap']["vma_end"]["start"]}
bss, {result[pids[0]]['bss']["vma_start"]["start"]} {result[pids[0]]['bss']["vma_end"]["start"]}, {result[pids[1]]['bss']["vma_start"]["start"]} {result[pids[1]]['bss']["vma_end"]["start"]}, {result[pids[2]]['bss']["vma_start"]["start"]} {result[pids[2]]['bss']["vma_end"]["start"]}
bss, {result[pids[0]]['bss']["vma_start"]["end"]} {result[pids[0]]['bss']["vma_end"]["start"]}, {result[pids[1]]['bss']["vma_start"]["start"]} {result[pids[1]]['bss']["vma_end"]["start"]}, {result[pids[2]]['bss']["vma_start"]["start"]} {result[pids[2]]['bss']["vma_end"]["start"]}
data, {result[pids[0]]['data']["vma_start"]["start"]} {result[pids[0]]['data']["vma_end"]["start"]}, {result[pids[1]]['data']["vma_start"]["start"]} {result[pids[1]]['data']["vma_end"]["start"]}, {result[pids[2]]['data']["vma_start"]["start"]} {result[pids[2]]['data']["vma_end"]["start"]}
data, {result[pids[0]]['data']["vma_start"]["end"]} {result[pids[0]]['data']["vma_end"]["start"]}, {result[pids[1]]['data']["vma_start"]["start"]} {result[pids[1]]['data']["vma_end"]["start"]}, {result[pids[2]]['data']["vma_start"]["start"]} {result[pids[2]]['data']["vma_end"]["start"]}
text, {result[pids[0]]['text']["vma_start"]["start"]} {result[pids[0]]['text']["vma_end"]["start"]}, {result[pids[1]]['text']["vma_start"]["start"]} {result[pids[1]]['text']["vma_end"]["start"]}, {result[pids[2]]['text']["vma_start"]["start"]} {result[pids[2]]['text']["vma_end"]["start"]}
text, {result[pids[0]]['text']["vma_start"]["end"]} {result[pids[0]]['text']["vma_end"]["start"]}, {result[pids[1]]['text']["vma_start"]["start"]} {result[pids[1]]['text']["vma_end"]["start"]}, {result[pids[2]]['text']["vma_start"]["start"]} {result[pids[2]]['text']["vma_end"]["start"]}
"""
print(result_csv)
