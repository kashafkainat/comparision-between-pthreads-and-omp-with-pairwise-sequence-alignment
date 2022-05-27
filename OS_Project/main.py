import matplotlib.pyplot as plt

method = []
time = []

f = open('output.txt','r')
for row in f:
	row = row.split(' ')
	method.append(row[0])
	time.append(float(row[1]))

plt.bar(method, time, color = 'g', label = 'File Data')

plt.xlabel('METHOD')
plt.ylabel('TIME')

plt.title('COMPARISON')
plt.legend()
plt.show()
