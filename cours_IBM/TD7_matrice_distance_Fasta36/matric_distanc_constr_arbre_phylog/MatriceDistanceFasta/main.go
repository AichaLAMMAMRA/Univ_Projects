// MatriceDistanceFasta project main.go
package main

import (
	"MatriceDistanceFasta"
	"flag"
)

var configPath = flag.String("config", "config.ini", "path of config fil .ini")

func main() {
	var o configDistance
	flag.Parse()
	if (*configPath) != "" {
		o = MtriceDistFasta.ReadConfigIni(*configPath)
	}
	bacterias := MtriceDistFasta.ReadConfig(o.infile)
	log.Println("CalculateDistance")
	matrix := MtriceDistFasta.CalculateDistance(bacterias, o)
	MtriceDistFasta.PrintMatrix(matrix)
	MtriceDistFasta.SaveToCsv(matrix, bacterias, o.outfile)

}
