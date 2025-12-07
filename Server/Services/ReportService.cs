using Server.Data;
using Server.DTOs;
using Server.Models;
using Server.Services.Interfaces;

namespace Server.Services
{
    public class ReportService : IReportService
    {
        private readonly AppDbContext _context;

        public ReportService(AppDbContext context)
        {
            _context = context;
        }

        public async Task ReportMessageAsync(string userId, ReportMessageDto dto)
        {
            var report = new MessageReport
            {
                ReporterId = userId,
                MessageId = dto.MessageId,
                Reason = dto.Reason,
                ReportedAt = DateTime.UtcNow
            };

            _context.MessageReports.Add(report);
            await _context.SaveChangesAsync();
        }
    }
}
