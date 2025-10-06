namespace Server.Services.Interfaces
{
    public interface IChatService
    {
        Task<bool> PinMessageAsync(string userId, int chatId, int messageId);
    }
}
