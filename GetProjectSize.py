import os

cwd = os.getcwd()
search_directories = f'{cwd}\\res', f'{cwd}\\src'

type_whitelist = 'cpp', 'h', 'shader'
file_blacklist = 'stb_image.h'
directory_blacklist = [f'{cwd}\\src\\vendor\\glm',]

array = [((file_path := f'{path}\\{file_name}'), len((file_data := open(file_path, 'r').readlines())), file_data.count('\n')) for search_directory in search_directories for path, _, files in os.walk(search_directory) if not True in [directory in path for directory in directory_blacklist] for file_name in files if file_name not in file_blacklist and file_name.split('.')[-1:][0] in type_whitelist]
input('\n'.join(f'{file_name}: {total}, {dead}' for file_name, total, dead in array) + '\nUnique: ' + str((unique := sum(total - dead for _, total, dead in array))) + '\nTotal: ' + str((total := sum(total for _, total, _ in array))) + f'\nDead Space: {round(100 - 100 * unique / total)}%')

