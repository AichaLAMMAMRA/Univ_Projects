// Linkagepack project Linkagepack.go
package Linkagepack

import (
	"bufio"
	"bytes"
	"compress/gzip"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"strconv"
)

func zip(file_name, s string) int64 {
	fmt.Println("-- Début zippage " + file_name + " --")
	var buf bytes.Buffer
	zw := gzip.NewWriter(&buf)
	zw.Name = (file_name + ".gzip")

	//Ecriture du fichier zippé
	_, err := zw.Write([]byte(s))
	if err != nil {
		log.Fatal(err)
	}

	err = ioutil.WriteFile(file_name+"_.gz", buf.Bytes(), 0666)

	//Recupération des stats sur le fichier
	fi, err := os.Stat(file_name + "_.gz")
	if err != nil {
		log.Fatal(err)
	}

	//Récupération de la taille du fichier zippé
	taille_zip := fi.Size()
	if err := zw.Close(); err != nil {
		log.Fatal(err)
	}

	fmt.Println("-- Fin zippage " + file_name + " --")
	os.Remove(file_name + "_.gz")
	return taille_zip
}

func Affiche_matrice(matrice [][]float64) {
	for i := 0; i < 15; i++ {
		fmt.Println(matrice[i])
	}
}

func Ecrit_matrice(matrice [][]float64) {
	var matrice_string string
	for i := 0; i < 15; i++ {
		for j := 0; j < 15; j++ {
			matrice_string = matrice_string + strconv.FormatFloat(matrice[i][j], 'f', 6, 64) + " "
		}
		matrice_string = matrice_string + "\n"
	}
	fmt.Println("Resultat :\n" + matrice_string)
	content := []byte(matrice_string)
	ioutil.WriteFile("matrice_distance.txt", content, 0640)
}

func Distance(file_name1, file_name2, A, B string) float64 {
	fmt.Printf("-- Debut calcul distance(%s , %s) --\n", file_name1, file_name2)
	taille_Zip_A := zip(file_name1, A)
	taille_Zip_B := zip(file_name2, B)
	taille_Zip_AB := zip("A&B", A+B)
	taille_Zip_AA := zip("A&A", A+A)
	taille_Zip_BB := zip("B&B", B+B)
	fmt.Printf("-- Fin calcul distance(%s , %s) --\n", file_name1, file_name2)
	fmt.Printf("(A = %d, B = %d, A&B = %d, A&A = %d, B&B = %d)\n", taille_Zip_A, taille_Zip_B, taille_Zip_AB, taille_Zip_AA, taille_Zip_BB)
	var d float64
	op1 := float64(float64(taille_Zip_AB) / float64((taille_Zip_A + taille_Zip_B)))
	op2 := float64(float64(taille_Zip_AA) / float64((4 * taille_Zip_A)))
	op3 := float64(float64(taille_Zip_BB) / float64((4 * taille_Zip_B)))
	fmt.Printf("op1 = %f, op2 = %f, op3 = %f\n", op1, op2, op3)
	d = op1 - op2 - op3
	if d < 0 {
		return -d
	}
	return d
}

func ProcessFile(s string) string {
	var file_string string

	_, err := os.Stat(s)
	if err != nil {
		fmt.Fprintf(os.Stderr, "the file %s doesn't exist! \n", s)
		os.Exit(1)
	}

	file, _ := os.Open(s)

	reader := bufio.NewReader(file)
	scanner := bufio.NewScanner(reader)

	var lineNumber int
	var shortTreatment bool

	next_line_is_dna := false
	for scanner.Scan() {
		lineNumber++

		if shortTreatment && (lineNumber > 40000) {
			break
		}
		//var val_str string
		val_str := scanner.Text()
		if len(val_str) > 0 {
			if val_str[0] == '>' {
				next_line_is_dna = true
				continue
			} else if next_line_is_dna {
				file_string = file_string + val_str
				continue
			}
		}
	}

	return file_string
}

func min_matrice(matrice [][]float64, taille int) (int, int, float64) {
	min := matrice[0][1]
	i_min := 0
	j_min := 1
	for i := 0; i < taille; i++ {
		for j := i; j < taille; j++ {
			if matrice[i][j] < min && i != j {
				min = matrice[i][j]
				i_min = i
				j_min = j
			}
		}
	}

	return i_min, j_min, min
}

/*
func single_linkage(resultats [][]float64) {
	var i_min int
	var j_min int
	var min float64
	//var buffer = " "
	//tree := []string
	taille := len(resultats)

	var matrix_tmp [][]float64
	matrix_tmp = make([][]float64, taille, taille)

	for i := 0; i < taille; i++ {
		matrix_tmp[i] = make([]float64, taille)
		//fmt.Println("-- --", resultats[i])
	}

	for taille > 2 {
		for i := 0; i < (taille - 1); i++ {
			for j := i;  j < taille; j++ {
				if resultats[i][j] < min && i != j {
					min = resultats[i][j]
					i_min = i
					j_min = j
				} else {
					continue
				}
			}
		}

		for i := 0; i < taille; i++ {
			for j := 0; j < taille; j++ {

				if j == Min(i_min, j_min) {
					resultats[i][j] = math.Min(resultats[i][j], resultats[i][j_min])
				} else {
					continue
				}
				if i == Min(i_min, j_min) {
					resultats[i][j] = math.Min(resultats[i][j], resultats[j_min][j])
				} else {
					continue
				}

			}
		}
		taille = taille - 1
		//------------------
		/buffer = buffer + "("+ i_min +":"+resultats[i_min][j_min]+","+ j_min + ":"+resultats[i_min][j_min]+")"
		tree = "(" + buffer + ")" + ";"
		f, _ := os.Create("single_linkage_TD3.txt")
		defer f.Close()
		fmt.Fprint(f, "tree: \n ")
		fmt.Fprint(f, tree[0], "\n")
		/
		//---------------
	}
	fmt.Println("--Apres le regroupement la matrice est de taille --", taille)
}
*/

func Linkage(matrice [][]float64, noms []string, taille int) {
	chaine := ""
	var i_min int
	var j_min int
	var min float64

	var old_index []int
	var dist []float64
	old_index = make([]int, taille)
	dist = make([]float64, taille)

	for i, _ := range old_index {
		old_index[i] = i
		dist[i] = 0
	}

	for i := 0; i < len(old_index); i++ {
		if i != 1 {
			i_min, j_min, min = min_matrice(matrice, old_index[i])
			min_col, max_col := min_max(i_min, j_min)
			columns_merge(matrice, &old_index, min_col, max_col)
			write_name(noms, dist, min_col, max_col, min)
			//-------------------------------------------------------
			if i == 0 {
				chaine = chaine + noms[min_col]
			} else {
				chaine = chaine + "," + noms[min_col]
			}
			//-------------------------------------------------------
		}
	}

	/* au debut on a fait :
	chaine = noms[0] + ";"
	et cette commande recupere uniquementle premier arbre
	alors que il fallait faire une concatenation qu'on vient d'ajouter
	afin d'ecrire tous les arbres avant le terminateur point virgule */

	//chaine = noms[0] + ";"
	chaine = chaine + ";"

	content := []byte(chaine)
	ioutil.WriteFile("chaine_netwick.txt", content, 0640)
}

func columns_merge(matrice [][]float64, old_index *[]int, min_col, max_col int) {
	for _, i_old := range *old_index {
		if i_old != min_col && i_old != max_col {
			matrice[i_old][min_col] = fmini(matrice[i_old][min_col], matrice[i_old][max_col])
			matrice[min_col][i_old] = matrice[i_old][min_col]
		}
	}

	var new_index int
	for i, v := range *old_index {
		if v == max_col {
			new_index = i
			break
		}
	}

	for i, _ := range (*old_index)[new_index : len(*old_index)-1] {
		ind := i + new_index
		(*old_index)[ind] = (*old_index)[ind+1]
	}

	*old_index = (*old_index)[:len(*old_index)-1]
}

func write_name(noms []string, dist []float64, min_col, max_col int, min float64) {
	noms[min_col] = fmt.Sprintf("(%s:%2f,%s:%2f)", noms[min_col], min-dist[min_col], noms[max_col], min-dist[max_col])
	dist[min_col] = min
}

func fmini(x, y float64) float64 {
	if x < y {
		return x
	} else {
		return y
	}
}

func min_max(x, y int) (int, int) {
	if x < y {
		return x, y
	} else {
		return y, x
	}
}
