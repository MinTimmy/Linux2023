memory_format = dict()
memory_dict = dict()
memory_list = list()
picture = list()
file_path = "output"
with open(file_path, 'r') as file:
    for line in file:
        name, addr = line.split(':')
        # if "vma" in name:
        #     continue
        addr = addr.strip()
        memory_dict[name] = addr
        memory_list.append((addr,name))
        addr = addr.strip()
        tmp = name.split('.')
        if len(tmp) == 3:
            thread, struct, position = tmp
            memory_format.setdefault(thread, {}).setdefault(struct, {})[position] = addr
        elif len(tmp) == 4:
            thread, struct, position, sub_position = tmp
            memory_format.setdefault(thread, {}).setdefault(struct, {}).setdefault(position, {})[sub_position] = addr
memory_list = sorted(memory_list, key=lambda x: int(x[0], 16), reverse=True)
result_dict = {}

for item in memory_list:
    name, addr = item
    name_li = addr.split('.')
    if name not in result_dict:
        result_dict[name] = {
            "main":{
                "mm":{
                },
                "vma":{
                }
            },
            "thread_1":{
                "mm":{
                },
                "vma":{
                }
            },
            "thread_2":{
                "mm":{
                },
                "vma":{
                }
            }
        }
    result_dict.setdefault(name, {}).setdefault(name_li[0], {}).setdefault(name_li[1], {})[name_li[2]] = addr


for name in result_dict:
    print(name, end=',')
    for thread in result_dict[name]:
        for struct in result_dict[name][thread]:
            for position in result_dict[name][thread][struct]:
                print(result_dict[name][thread][struct][position], end=' ')
            print(end=',')
    print()