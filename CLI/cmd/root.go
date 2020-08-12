package cmd

import (
	"github.com/spf13/cobra"
)

var rootCmd = &cobra.Command{
	Use:   "",
	Short: "",
	Long:  "",
}

//Execute Execute
func Execute() error {
	return rootCmd.Execute()
}
func init() {
	rootCmd.AddCommand(wordCmd)
}
