using Microsoft.EntityFrameworkCore;
using Server.Data;
using Server.DTOs;
using Server.Models;
using Server.Services.Interfaces;

namespace Server.Services
{
    public class MessageService : IMessageService
    {
        private readonly AppDbContext _context;

        public MessageService(AppDbContext context)
        {
            _context = context;
        }


        public async Task<bool> DeleteAsync(int userId, int messageId)
        {
            var message = await _context.Messages.FindAsync(messageId);
            if (message == null || message.SenderId != userId) return false;

            _context.Messages.Remove(message);
            await _context.SaveChangesAsync();
            return true;
        }

        public async Task<bool> EditAsync(int userId, int messageId, EditMessageDto dto)
        {
            var message = await _context.Messages.FindAsync(messageId);
            if (message == null || message.SenderId != userId) return false;

            message.Content = dto.NewContent;
            await _context.SaveChangesAsync();
            return true;
        }

        public async Task<Message> GetByIdAsync(int messageId)
        {
            return await _context.Messages.FindAsync(messageId);
        }

        public async Task<IEnumerable<Message>> GetChatHistoryAsync(int senderId, int receiverId)
        {
            return await _context.Messages
                .Where(m => (m.SenderId == senderId && m.ReceiverId == receiverId) ||
                            (m.SenderId == receiverId && m.ReceiverId == senderId))
                .OrderBy(m => m.Timestamp)
                .ToListAsync();
        }

        public async Task SendAsync(int senderId, MessageDto dto)
        {
            var message = new Message
            {
                SenderId = senderId,
                ReceiverId = dto.ReceiverId,
                Content = dto.Content,
                Timestamp = DateTime.UtcNow
            };

            _context.Messages.Add(message);
            await _context.SaveChangesAsync();
        }


        public async Task ForwardAsync(int senderId, ForwardMessageDto dto)
        {
            var message = new Message
            {
                SenderId = dto.SenderId,
                ReceiverId = dto.ReceiverId,
                Content = dto.Content,
                IsForwarded = dto.IsForwarded,
                Timestamp = DateTime.UtcNow
            };

            _context.Messages.Add(message);
            await _context.SaveChangesAsync();
        }
    }
}
