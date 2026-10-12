import os
import pathlib
import hashlib
import pandas as pd
from tabulate import tabulate

file_info = []
pd.set_option('display.max_rows', None)
pd.set_option('display.max_columns', None)
pd.set_option('display.max_colwidth', None)

def hash_file(path):
    hash = ""
    try:
        with open(path, "rb") as f:
            digest = hashlib.file_digest(f, "sha256")
            hash = digest.hexdigest()
    except PermissionError:
        print(f"Error: Permission denied for {path}")
    return hash

def recursive_dir_scan(directory):
    try:
        for entry in os.scandir(directory):
            if entry.is_dir():
                recursive_dir_scan(entry.path)
            if entry.is_file():
                hash = hash_file(entry.path)
                if len(hash) > 0:
                    file_info.append({"name":entry.name,
                                    "path":entry.path,
                                    "size (Bytes)":entry.stat().st_size,
                                    "hash": hash})
    except PermissionError:
        print(f"Permission denied: cannot access {directory}")

    df = pd.DataFrame(file_info)
    return df
    

def main():
    directory = pathlib.Path(input("Please enter a directory to search: "))
    files = recursive_dir_scan(directory)
    dupes = files.groupby("hash", as_index=False).filter(lambda x : len(x) > 1)
    dupes = dupes.groupby("hash")
    i = 1
    reclaimable_disk = 0

    for name, group in dupes:
        subset = group.drop(columns=["hash"])
        reclaimable_disk += subset["size (Bytes)"].sum() - subset.iloc[1, 2]
        print(f"Group {i}")
        print(tabulate(subset, headers="keys", tablefmt="psql"))
        print("")
        print("")
        i += 1

    print(f"Reclaimable Disk Space: {reclaimable_disk} Bytes")
    

if __name__ == "__main__":
    main()