Import("env")
import shutil, os

def copy_hex(source, target, env):
    src = str(target[0])
    dst = os.path.join(env["PROJECT_DIR"], "firmware.hex")
    shutil.copyfile(src, dst)
    print("Copied %s -> %s" % (src, dst))

env.AddPostAction("$BUILD_DIR/${PROGNAME}.hex", copy_hex)
