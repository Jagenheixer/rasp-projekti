import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 1. LUODAAN TESTIDATA (6 klusteria: X+, X-, Y+, Y-, Z+, Z-)
K = 6                   # klustereiden lukumäärä
N_per_cluster = 100     # pisteitä per klusteri
noise = 70              # hajonta

# Data sisältää myös klusteri-ID:n neljännessä sarakkeessa
data = np.zeros((K*N_per_cluster, 4))
data[0:100, :] = np.array([1800, 1500, 1500, 0])
data[100:200, :] = np.array([1200, 1500, 1500, 1])
data[200:300, :] = np.array([1500, 1800, 1500, 2])
data[300:400, :] = np.array([1500, 1200, 1500, 3])
data[400:500, :] = np.array([1500, 1500, 1800, 4])
data[500:600, :] = np.array([1500, 1500, 1200, 5])

# Lisätään satunnaista kohinaa
data[:, 0:3] += noise * np.random.rand(K*N_per_cluster, 3)

X_data = data[:, 0:3]   # 3D-koordinaatit
y_data = data[:, 3]     # klusteri-ID:t
N = K * N_per_cluster   # kokonaispisteiden määrä

# 2. APUFUNKTIOT
def distance_3d(a, b):
    """Laskee euklidisen etäisyyden kahden 3D-pisteen välillä."""
    return np.linalg.norm(a - b)

# 3. K-MEANS: ALUSTUS
Min = int(np.min(X_data))
Max = int(np.max(X_data))
centroids = np.random.randint(Min, Max, size=(K, 3))

# Varataan taulukot Step 2:lle
sums = np.zeros((K, 3)) # summataan pisteet voittajaa kohti
counts = np.zeros(K)    # lasketaan kuinka monta pistettä kukin klusteri voittaa
distances = np.zeros(K) # etäisyys kuhunkin centroidiin

# 4. K-MEANS: ITERAATIOT
num_iterations = 10

for iteration in range(num_iterations):
    sums[:] = 0
    counts[:] = 0

    # STEP 1: FIND WINNER FOR EACH POINT
    for row in X_data: # Lasketaan etäisyys kaikkiin centroidiin
        for c in range(K):
            distances[c] = distance_3d(row, centroids[c]) # Valitaan lähin centroid = voittaja
        winner = np.argmin(distances) # Lisätään piste summiin ja kasvatetaan laskuria
        sums[winner] += row
        counts[winner] += 1

    # STEP 2: UPDATE CENTROIDS
    for c in range(K):
        if counts[c] > 0: # Uusi keskipiste = kaikkien voittopisteiden keskiarvo
            centroids[c] = sums[c] / counts[c]
        else: # Jos klusterilla ei yhtään pistettä, arvotaan uusi satunnainen keskipiste
            centroids[c] = np.random.randint(Min, Max, size=(3,))

    print(f"Iteraatio {iteration}, centroidit:\n{centroids}\n") # Tulostetaan centroidien paikat iteraation jälkeen

# 5. PIIRRETÄÄN DATABILVI KLUSTEREITTAIN
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Värit klustereille
colors = ['r', 'g', 'b', 'k', 'm', 'y']

# Määritetään jokaiselle pisteelle voittajaklusteri labels-taulukkoon
labels = np.zeros(N, dtype=int)
for i, row in enumerate(X_data):
    dists = np.array([distance_3d(row, c) for c in centroids])
    labels[i] = np.argmin(dists)

# Piirretään pisteet klustereittain
for k in range(K):
    cluster_points = X_data[labels == k]
    ax.scatter(cluster_points[:, 0], cluster_points[:, 1], cluster_points[:, 2],
               s=15, color=colors[k], label=f"Cluster {k}")

for k in range(K):
    ax.scatter(centroids[k, 0], centroids[k, 1], centroids[k, 2],
               s=500, color=colors[k], marker='X', edgecolor='k')

ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")
plt.title("Kiihtyvyysdata klusteroinnin jälkeen")
plt.show()

original_centers = np.array([
    [1800, 1500, 1500],  # X+
    [1200, 1500, 1500],  # X-
    [1500, 1800, 1500],  # Y+
    [1500, 1200, 1500],  # Y-
    [1500, 1500, 1800],  # Z+
    [1500, 1500, 1200],  # Z-
])
# Tulostetaan centroidit ja lähin alkuperäinen klusteri
print("Centroidit ja lähin alkuperäinen klusteri:")
for i, centroid in enumerate(centroids): # Lasketaan etäisyydet kaikkiin alkuperäisiin keskuksiin
    dists = np.linalg.norm(original_centers - centroid, axis=1)
    closest = np.argmin(dists)
    print(f"Centroid {i}: {centroid}, lähin alkuperäinen klusteri: {closest} ({original_centers[closest]})")


# 6. TULOSTETAAN KLUSTEREIDEN KOKO
num_clusters = centroids.shape[0]
#Luodaan taulukko keskipisteet.h-tiedostoa varten
CP = np.zeros((num_clusters, 3), dtype=int)

#Käydään läpi keskipisteet ja määritetään minimi ja maksimi arvot
for i, centroid in enumerate(centroids):
    # Poimitaan pienin ja suurin arvo riviltä
    min_val = int(np.min(centroid))
    max_val = int(np.max(centroid))

# Määritetään aktiivinen sarake (X, Y tai Z) johon arvo asetetaan taulukkoon
    axis = (i // 2) % 3  # 0=X, 1=Y, 2=Z

    # Asetetaan pienin tai suurin arvo aktiiviselle akselille 
    if i % 2 == 0:
        CP[i, axis] = min_val   # parillinen = pieni arvo
    else:
        CP[i, axis] = max_val   # pariton = suuri arvo

# Tallenna C-headeriin
with open("keskipisteet.h", "w") as f:
    f.write("// Tämä tiedosto generoitu Pythonilla\n")
    #Kirjoitetaan keskipisteet taulukkoon
    f.write("int CP[{}][3] = {{\n".format(num_clusters))
    
    for i, row in enumerate(CP):
        line = "    {{{}, {}, {}}}".format(row[0], row[1], row[2])
        if i != num_clusters-1:
            line += ","
        line += "  // Keskipiste {}\n".format(i+1)
        f.write(line)
    
    f.write("};\n")

#Varmuustulostus
for i, row in enumerate(CP):
    print(f"Keskipiste {i+1}: {row}")
