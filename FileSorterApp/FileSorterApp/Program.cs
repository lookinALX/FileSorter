// See https://aka.ms/new-console-template for more information

using FileSorterApp;

var fileOrganizer = new FileOrganizer();

const string testDirectory = @"D:\projects\FileSorter\FileSorterApp\FileSorterApp.Tests\TestDirectory";

/*fileOrganizer.CreateDirectoriesBasedOnFileExtensions(testDirectory, 
    FileSystemHelper.GetFiles(testDirectory).ExtractFileExtensions());*/

FileSystemHelper.GetFiles(testDirectory).SplitDateTimeInMonthAndYear();