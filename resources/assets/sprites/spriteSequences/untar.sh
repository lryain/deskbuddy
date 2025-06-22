for file in *.tar; do
    # 提取文件名（不含扩展名）作为目录名
    dir="${file%.tar}"
    
    # 创建目录（若不存在）
    mkdir -p "$dir"
    
    # 解压到对应目录
    tar -xf "$file" -C "$dir"
done
