// estimateurPI_test
package estimateurPI

import (
	_ "encoding/csv"
	"fmt"
	_ "io/ioutil"
	_ "os"
	"testing"
)

func TestglideWindow(t *testing.T) {
	total := glideWindow([]byte("0126011"))
	fmt.Printf("result %d \n", total)

	tota := glideWindow([]byte("0"))
	fmt.Printf("result %d \n", tota)

}

func TestWriteGlideWindow(t *testing.T) {
	wind := windowDecimal{}
	err := wind.WriteGlideWindow()
	fmt.Printf("result %v\n", err)

	/*w := csv.NewWriter(os.Stdout)

	err = w.Error()
	_ = wind.WriteGlideWindow()*/

}

func TestloadData(t *testing.T) {
	wind := windowDecimal{}
	_ = wind.loadData("C:/Users/hp/go/src/estimateurPI/ pi1000000")

	fmt.Printf("result  %v\n", wind.targetData)

}

func TestNewWindowsDecimal(t *testing.T) {
	total := NewWindowsDecimal(20, "C:/Users/hp/go/src/estimateurPI/ pi1000000")
	out, _ := total.gliderWindow()
	t.Logf("count %+v\n", total)
	t.Logf("count %+v\n", out)
}

func TestNewWindowsDecimal2(t *testing.T) {
	/*
		wind := windowDecimal{}
		_ = wind.loadData("C:/Users/AichaLam/go/src/estimateurPI/ pi1000000")

		tot := NewWindowsDecimal2(40, string(wind.targetData))
		t.Logf("count %v \n", tot.targetData)
	*/
}

func TestNewWindowsDecimal4(t *testing.T) {
	/*total := NewWindowsDecimal(60, "C:/Users/AichaLam/go/src/estimateurPI/ pi100000")
	out, _ := total.gliderWindow() //
	t.Logf("count %+v\n", total)
	t.Logf("count %+v\n", out)*/
}

func TestNewWindowsDecimal3(t *testing.T) {
	/*total := NewWindowsDecimal(180, "C:/Users/AichaLam/go/src/estimateurPI/ pi100000")
	out, _ := total.gliderWindow()
	t.Logf("count %+v\n", total)
	t.Logf("count %+v\n", out)*/
}

func TestEqual(t *testing.T) {
	a := resultWindow{1, 5, 1, 1, 2, 1, 2}
	b := resultWindow{1, 5, 1, 1, 2, 2, 2}

	//t.Logf("count %+v\n", b)

	if !Equal(a, b) {

		t.Errorf("function Equal is not working - different value case \n")
	}
}

func TestComputeCount(t *testing.T) {
	a := []int{1, 5, 1, 0, 1, 2, 1, 2}
	aResult := resultWindow{1, 4, 2, 0, 0, 1}
	aCompute := computeCount(a)
	t.Logf("count %+v\n", aCompute)
	if !Equal(aCompute, aResult) {
		t.Errorf("Error in output %+v\n", aCompute)
	}
}

var result int

//run a fixed size
func BenchmarkGlideWindow(b *testing.B) {
	// run the glideWindow function b.N times
	var total int
	for n := 0; n < b.N; n++ {
		total = glideWindow([]byte("010101"))
	}

	result = total
}

var slice []byte

func benchmarkGlideWindow2(i int, b *testing.B) {
	// run the glideWindow function b.N times
	for n := 0; n < b.N; n++ {
		//slice := make([]byte, i)
		glideWindow(slice)
	}
}

func BenchmarkGlideWindow3(b *testing.B) {
	slice = make([]byte, 2000)
	benchmarkGlideWindow2(2000, b)
}
