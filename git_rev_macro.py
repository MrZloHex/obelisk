import subprocess

fw_sha = subprocess.check_output(["git", "rev-parse", "--short", "HEAD"]).strip()

with open('Inc/firmware_metadata.h', 'r') as file:
    data = file.readlines()

print(data)

data[3] = '#define FW_SHA "%s"\n' % (fw_sha.decode())

with open('Inc/firmware_metadata.h', 'w') as file:
    file.writelines( data )
