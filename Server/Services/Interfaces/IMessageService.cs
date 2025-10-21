using Server.DTOs;
using Server.Models;

namespace Server.Services.Interfaces
{
    public interface IMessageService
    {
        Task SendAsync(int senderId, MessageDto dto);
        Task<IEnumerable<Message>> GetChatHistoryAsync(int senderId, int receiverId);
        Task<bool> DeleteAsync(int userId, int messageId);
        Task<bool> EditAsync(int userId, int messageId, EditMessageDto dto);
        Task<Message> GetByIdAsync(int messageId);
    }
}
