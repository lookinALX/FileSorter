namespace FileSorterApp;

public static class FileSystemHelper
{
    public static void CreateDirectory(string directory)
    {
        Directory.CreateDirectory(directory);
    }

    public static string[] GetFiles(string directory)
    {
        var files = Directory.GetFiles(directory);
        return files;
    }

    public static string[] ExtractFileExtensions(string[] files)
    {
        return files.Select(file => Path.GetExtension(file).TrimStart('.'))
            .Where(ext => !string.IsNullOrEmpty(ext))
            .ToArray();
    }
}