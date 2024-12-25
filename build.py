import os
import platform
import shutil
import subprocess
import sys


def define_values():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    values = {
        "generator": "Ninja",
        "outputDirectory": f"{script_dir}/out",
        "buildDirectory": f"{script_dir}/out/generated",
        "installDirectory": f"{script_dir}/out/athylen",
        "curl_curlDirectory": f"null",
        "yaml_yamlDirectory": f"null",
        "cthreads_cthreadsDirectory": f"null",
        "cjson_cjsonDirectory": f"null",
        "target": platform.system().lower(),
        "config_configuration": "debug"
    }

    return values


def sub_argument_process(args):
    values = define_values()
    # Every value with a name before an underscore will be available
    if len(args) == 0:
        return values

    available_values = {}
    for value in values:
        split_value = value.split("_")
        if len(split_value) > 1:
            available_values[split_value[0]] = split_value[1]

    flags = []
    for arg in args:
        split_arg = arg.split('=')

        if split_arg[0][:2] != "--":
            continue

        if len(split_arg) == 1:
            flags.append(arg)
            continue

        if split_arg[0][2:] in available_values:
            value = f"{split_arg[0][2:]}_{available_values[split_arg[0][2:]]}"
            values[value] = split_arg[1]

    if len(flags) > 0:
        for flag in flags:
            values[flag] = True
    return values



def execute_command(cmd_str, short_mode):
    running_string = "\033[34mRUNNING: \033[0m"
    success_string = "\033[32mSUCCEED: \033[0m"
    error_string = "\033[31mERROR: \033[0m"
    result_string ="\033[33mRESULT: \033[0m"

    print(f"{running_string}{cmd_str}")

    result = subprocess.run(cmd_str, capture_output=short_mode, shell=True)
    if not short_mode:
        result_value = "\033[31mERROR\033[0m"
        if result.returncode == 0:
            result_value = "\033[32mSUCCESS\033[0m"
        print(f"{result_string}{result_value}")
        return

    decoded_string = result.stdout.decode('iso-8859-1')
    if result.returncode == 0:
        print(f"{success_string}{decoded_string}")
        return
    print(f"{error_string}{decoded_string}")



def remove_directory(dir_str, working_dir, notice):
    os.chdir(working_dir)
    success_string = "\033[32mSUCCEED: \033[0m"
    error_string = "\033[31mERROR: \033[0m"
    try:
        shutil.rmtree(dir_str)
        print(f"{success_string} Removed {dir_str} directory.")
    except Exception as e:
        if isinstance(e, FileNotFoundError):
            if notice:
                print(f"{error_string} There isn\'t a {dir_str} directory at {working_dir}.")
            return
        if notice:
            print(f"{error_string} Failed to remove {dir_str} directory at {working_dir}: {e}")

def specified_action(args, values, script_dir):
    if len(args) > 0:
        if args[0] == "clean":
            remove_directory(values["outputDirectory"], script_dir, True)
            return True
        if args[0] == "rebuild":
            remove_directory(values["outputDirectory"], script_dir, False)
            return False



def execute_specified_action(args, values, script_dir):
    executed = specified_action(args, values, script_dir)
    return executed



def get_architecture():
    arch = platform.processor().split(" ")[0].lower()
    if arch == "amd64":
        arch = "x64"
    return arch



def get_target(values):
    ops = values['target']
    if ops == "windows":
        return "win"
    return ops



def set_install_directory(values, arch, target):
    values["installDirectory"] = (f"{values['outputDirectory']}/"
                                  f"{target}-{arch}-{values['config_configuration']}")



def get_build_type(build_type):
    build_type = build_type.capitalize()
    if build_type == "Test":
        build_type = "RelWithDebInfo"
    elif build_type not in ["Debug", "Release"]:
        print_error(f"Build configuration {build_type} is not supported, debug will be used instead.")
        return "Debug"
    return build_type



def print_error(message):
    error_string = "\033[31mERROR: \033[0m"
    print(f"{error_string} {message}")



def run_cmake_command(values, short_mode):
    execute_command(f"cmake "
                    f"-G Ninja "
                    f"-DCMAKE_LINKER=lld "
                    f"-DCMAKE_INSTALL_PREFIX=\"{values['installDirectory']}\" "
                    f"-B \"{values['buildDirectory']}\" "
                    f"-DCMAKE_BUILD_TYPE={get_build_type(values['config_configuration'])} "
                    f"-DCJSON_DIR=\"{values['cjson_cjsonDirectory']}\" "
                    f"-DCTHREADS_DIR=\"{values['cthreads_cthreadsDirectory']}\" "
                    f"-DYAML_DIR=\"{values['yaml_yamlDirectory']}\" "
                    f"-DCURL_DIR=\"{values['curl_curlDirectory']}\" ", short_mode)



def build_and_install(values, short_mode):
    os.chdir(values["buildDirectory"])
    execute_command("cmake --build .", short_mode)
    execute_command(f"cmake --install . --prefix {values['installDirectory']}", short_mode)

def execute_programm(should_run, values, short_mode):
    if should_run:
        execute_command(f"{values['installDirectory']}/bin/cmount.exe", short_mode)


def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    args = sys.argv[1:]
    values = sub_argument_process(args)

    os.chdir(script_dir)

    if execute_specified_action(args, values, script_dir):
        return

    arch = get_architecture()
    target = get_target(values)

    set_install_directory(values, arch, target)


    short_mode = not values.keys().__contains__("--i")
    run_cmake_command(values, short_mode)

    build_and_install(values, short_mode)

    should_run = values.keys().__contains__("--run")
    execute_programm(should_run, values, short_mode)

if __name__ == '__main__':
    main()