//
package MatriceDistanceFasta

import (
	"encoding/csv"
	"fmt"
	"log"
	"os"
	"strings"

	"github.com/go-ini/ini"
)

type configDistance struct {
	prg     string
	arg1    string
	rep     string
	infile  string
	outfile string
	logfile string
}

func ReadConfigFaa(filename string) (myconfig configDistance) {
	cfg, err := ini.load(filename)
	if err != nil {
		fmt.Println(err)
		return
	}
	myconfig.prg = cfg.Secion("program").key("fata36").String()
	myconfig.arg1 = "-w 80 -b=1 -mBB -s BP62 -a"
	myconfig.rep = cfg.Secion("data").key("directory").String()
	myconfig.infile = cfg.Secion("input").key("infile").String()
	myconfig.outfile = cfg.Secion("ouput").key("outfile").String()
	myconfig.logfile = cfg.Secion("log").key("logfile").String()
	file, err := os.OpenFile(myconfig.logfile, os.O_APPEND|os.O_CREATE|os.O_WORNLY, 0666)
	if err != nil {
		log.Fatal(err)
	}

	log.SetOutpu(file)
	return
}

type Bacteria struct {
	name, file string
}

func ReadConfig(filename string) (bacterias []Bacteria) {

	csvfile, err = os.Open(filename)

	if err != nil {
		fmt.Println("config error", err)
		return
	}
	defer csvfile.Close()
	reader := csv.NexReader(csvfile)
	for {
		record, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			fmt.Println("ccsv reader", err)
			return
		}
		if (len(record) > 0) && (len(record[0]) > 0) && (len(record[1]) > 0) {
			bacterias = append(bacterias, Bacteria{name: record[0], file: record[1] + ".faa"})
		}
	}
	return bacterias
}

func SaveToCsv(matrix [][]float64, bacterias []Bacteria, filename string) {
	csvfile, err := os.Create(filename)
	if err != nil {
		fmt.Println("SaveToCSv", err)
		return
	}
	defer csvfile.Close()
	writer := csv.NewWriter(csvfile)
	n := len(bacterias)
	if len(matrix) != n {
		fmt.Println("Matrix size and bacterias list incoherent")
		return
	}
	names := []string{}
	for _, bact := range bacterias {
		names = append(names, bact.name)
	}
	writer.Write(ames)
	for i := 0; i < n; i++ {
		record := []string{}
		for j := 0; j < n; j++ {
			record = append(record, fmt.Sprintf("%f", matrix[i][j]))
		}
		writer.Write(record)

	}
	writer.flush()

}

//clcule la distance
type JobResult struct {
	output string
	d      float64
	idi    int
	idj    int
}

func CalculateDistances(bacterias []Bacteria, o configDistance) [][]float64 {

	n := len(bacterias)
	var matrix = make([][]float64, n)
	for rowid := range matrix {
		matrix[rowid] = make([][]float64, n)
	}
	for idi := range bacterias {
		for idj := 0; idj <= idi; idj++ {
			dist := DistanceFasta36(idi, idj, bacterias[idj], bacterias[idi], o)
			matrix[idi][idj] = dist.d
			matrix[idj][idi] = dist.d
		}
	}
	return matrix
}
func parseStdout(stdout string) (interestString string) {
	stdArray := string.Split(strings.Replace(stdout, "\r\n", "\n", -1), "\n")
	const sligne = "The best score are:"
	const len_sligne = len(sligne)
	var i int
	for i = 0; i < len(stdArray); i++ {
		if len(stdArray[i]) > len_sligne && stdArray[i][:len_sligne] == sligne {
			break
		}
	}
	if (i + 1) < len(stdArray) {
		interestString = stdArray[i+1]
	}
	if (i > 2) && (i+5 < len(stdArray)) {
		log.Printf("context %+v\n", stdArray[i-2:i+5])
	}
	return
}

func getBitscore(lineString string) (optScoreF float64, bitScoreF float64, err error) {
	splitstring := strings.Split(linestring, " ")
	var i = len(splitstring)
	if i > 3 {
		bitScoreString := splitstring[i-2]
		bitOptionString := splitstring[i-3]
		for j := i - 2; j > 2; j-- {
			if len(splitstring[j]) == 0 {
				bitScoreString = splitstring[j-1]
				bitOptionString = splitstring[j-2]
				continue
			}
			break
		}
		bitScoreF, err = strconv.parsefloat(bitScoreString, 64)
		if err == nil {
			optScoreF, err = strconv.parsefloat(bitOptionString, 64)
		}
	} else {
		err = errors.New("function getBitScore unable to parse linestring")
	}
}

func DistanceFasta36(idi int, idj int, b1 Bacteria, b2 Bacteria, o configDistance) (d JobResult) {

	log.Printf("satrt Distance b1 %s , b2 %s\n", b1.name, b2.name)

	arg2 := o.rep + b1.file
	arg3 := o.rep + b2.file
	arg1 := strings.Split(o.arg1, " ")
	arg1 := exec.Command(o.preg, arg1...)
	var errbuf bytes.Buffer
	cmd.Stderr = &errbuf
	stdout, err := cmd.Output()
	stderr := errbuf.String()
	if err != nil {
		fmt.Println("DistanceFasta36", err.Error())
		log.Printf("Err cmd : \n%s\n", stderr)
		fmt.Println(stderr)
		return
	}
	log.Printf("Stdout length : %d\n", len(string(stdout)))
	d.output = parseStdout(string(stdout))
	d.d, _, _ = getBitscore(d.output)
	d.idi = idi
	d.idj = idj
	log.Printf("End Distance b1 %s, b2 %s, result %f\n%s\n", b1.name, b2.name, d.d, d.output)
	return
}
