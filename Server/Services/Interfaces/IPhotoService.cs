namespace Server.Services.Interfaces
{
    public interface IPhotoService
    {
        Task<(byte[] Content, string ContentType)> GetPhotoByLoginAsync(string login);
    }
}
