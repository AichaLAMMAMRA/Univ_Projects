// main
package main

import (
	"Linkagepack"
	"fmt"
)

func main() {

	file_bac_sub := "NC_000964.fna"
	file_bac_amy := "NC_009725.fna"
	file_bac_pum := "NC_009848.fna"
	file_bac_thu := "NC_014171.fna"
	file_bac_cer := "NC_012472.fna"
	file_bac_ant := "NC_003997.fna"
	file_bac_coa := "NC_015634.fna"
	file_bac_atr := "NC_014639.fna"
	file_bac_lic := "NC_006322.fna"
	file_esc_col := "NC_000913.fna"
	file_pse_aer := "NC_011770.fna"
	file_rho_sph := "NC_009428.fna"
	file_str_fla := "NC_016114.fna"
	file_mic_lut := "NC_012803.fna"
	file_lac_lac := "NC_002662.fna"

	nb_fichiers := 15
	fmt.Println("nb_fichiers = ", nb_fichiers)

	tab := []string{file_bac_sub, file_bac_amy, file_bac_pum, file_bac_thu, file_bac_cer,
		file_bac_ant, file_bac_coa, file_bac_atr, file_bac_lic, file_esc_col,
		file_pse_aer, file_rho_sph, file_str_fla, file_mic_lut, file_lac_lac}

	noms := []string{"Bacillus_subtilis", "Bacillus_amyloliquefaciens_FZB42", "Bacillus_pumilus_SAFR_032",
		"Bacillus_thuringiensis_BMB171", "Bacillus_cereus_03BB102", "Bacillus_anthracis_Ames",
		"Bacillus_coagulans_2_6", "Bacillus_atrophaeus_1942", "Bacillus_licheniformis_ATCC_14580",
		"Escherichia_coli_K_12_substr__MG1655", "Pseudomonas_aeruginosa_LESB58", "Rhodobacter_sphaeroides_ATCC_17025",
		"Streptomyces_flavogriseus_ATCC_33331", "Micrococcus_luteus_NCTC_2665_uid59033", "Lactococcus_lactis_Il1403"}

	var resultats [][]float64
	resultats = make([][]float64, nb_fichiers, nb_fichiers)

	for i := 0; i < nb_fichiers; i++ {
		resultats[i] = make([]float64, nb_fichiers)
	}

	var chaines_adn []string
	chaines_adn = make([]string, nb_fichiers)

	fmt.Println("-- Debut des lectures des ADNs --")
	for i := 0; i < nb_fichiers; i++ {
		fmt.Printf("---- #%d Lecture de %s ----\n", i, tab[i])
		chaines_adn[i] = Linkagepack.ProcessFile(tab[i])
	}
	fmt.Println("-- Fin des lectures des ADNs --")

	for i := 0; i < nb_fichiers; i++ {
		for j := i; j < nb_fichiers; j++ {
			if i == j {
				resultats[i][j] = 0.0
			} else {
				resultats[i][j] = Linkagepack.Distance(tab[i], tab[j], chaines_adn[i], chaines_adn[j])
				resultats[j][i] = resultats[i][j]
				fmt.Println(resultats[i][j])
			}
		}
		fmt.Println("")
	}

	Linkagepack.Affiche_matrice(resultats)
	Linkagepack.Ecrit_matrice(resultats)
	Linkagepack.Linkage(resultats, noms, nb_fichiers)

}
