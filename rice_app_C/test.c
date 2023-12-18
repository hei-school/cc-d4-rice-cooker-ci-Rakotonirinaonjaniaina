import subprocess

//Function simulation programe

def simulate_rice_cooker():
    process = subprocess.Popen(["./your_rice_programe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, universal_newlines=True)

 //Simulation action

    process.stdin.write("1\n") 
    process.stdin.write("500\n") 
    process.stdin.write("10\n")  
    process.stdin.write("\n") 
    process.stdin.write("q\n") 

    process.stdin.flush()
    process.stdin.close()

    output = process.stdout.readlines()
    process.terminate()

    return output

//Execute default simulation

result = simulate_rice_cooker()
for line in result:
    print(line.strip())
