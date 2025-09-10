
import subprocess
import re
import json

# Run 7zip benchmark and capture output
output = subprocess.run(["7z", "b"], capture_output=True, text=True, check=True)
print(output)

# Extract total rating
score = int(re.findall(r'\d+', output.stdout)[-1])
print(score)

# Store in Bencher-style dictionary
result = { 
   "7z_score" : {
      "rating" : {
         "value" : score
      }
   }
}

# Save in file
file_out= "7z_score.json"
with open(file_out, "w") as f:
   json.dump(result, f, indent=4)