using Server.Data;
using Server.Models;
using Server.Services.Interfaces;

namespace Server.Services
{
    public class ChatService : IChatService
    {
        private readonly AppDbContext _context;

        public ChatService(AppDbContext context)
        {
            _context = context;
        }

        public async Task<bool> PinMessageAsync(string userId, int chatId, int messageId)
        {
            var chat = await _context.Set<Chat>().FindAsync(chatId);
            if (chat == null || chat.UserId != userId) return false;

            chat.PinnedMessageId = messageId;
            await _context.SaveChangesAsync();
            return true;
        }
    }
}
