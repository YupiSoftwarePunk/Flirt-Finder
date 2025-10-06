using Server.DTOs;
using Server.Models;

namespace Server.Services.Interfaces
{
    public interface IMessageService
    {
        Task SendAsync(string? senderId, MessageDto dto);
        Task<IEnumerable<Message>> GetChatHistoryAsync(string senderId, string receiverId);
        Task<bool> DeleteAsync(string userId, int messageId);
        Task<bool> EditAsync(string userId, int messageId, EditMessageDto dto);
        Task<Message> GetByIdAsync(int messageId);
    }
}
