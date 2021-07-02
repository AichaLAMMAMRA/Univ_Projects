# EstimateurPI

EstimateurPI est un programme golang qui recherche un motif en nombre pi. il essaie de savoir combien de fois une certaine fenêtre rencontrera un alphabet prédéfini (nous recherchons de 01 à 26 nombres entre 00 et 99 pi nombres décimaux).

 Test
pour tester les méthodes et les fonctions, des packages de tests ont été utilisés. Un premier test a été mis en place puis amélioré à l'aide d'une couverture de test. une couverture de test de 100% a été atteinte.
```bash
go test -coverprofile=coverage.out
go tool cover -html=coverage.out
```

### Benchmark 
Les fonctions a également été testée en terme de performances avec benchmark. La charge de données est ce qui coûte le plus. Toutes les fonctions testées sont comparées.

## Main 
Un fichier de 100 000 décimales a été utilisé pour vérifier les résultats.
```golang

a := estimateurPI.NewWindowsDecimal2(20, "C:PATH_TO_FILE\\estimateurPI\\pi100000.txt")
```
## Results
les résultats pour les fenêtres 20, 40, 80, 160,  sont enregistrés sous forme de fichier CSV à l'aide de la commande sur cmd:
```bash
type nul > output.csv
go run main.go >> output.csv
````
Les résultats montrent que la fréquence d'apparition de l'alphabet la plus élevée se produit avec une fenêtre de 20 pour 4 lettres. avec des fenêtres plus grandes, le motif diminue.


#Auteurs:
----------
*Aicha LAMMAMRA

* Date : 09/01/2021