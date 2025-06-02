namespace FileSorterApp;

public class FileOrganizer
{
    public void CreateDirectoriesBasedOnFileExtensions(string path, string[] extensions)
    {
        foreach (var ext in extensions)
        {
            FileSystemHelper.CreateDirectory(Path.Combine(path, ext));
        }
    }

    public void CreateDirectoriesBasedOnFileDates(string pathForNewDir, DateTime[] dates)
    {
        
    }
}