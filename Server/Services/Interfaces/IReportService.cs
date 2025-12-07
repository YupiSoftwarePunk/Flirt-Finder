using Server.DTOs;

namespace Server.Services.Interfaces
{
    public interface IReportService
    {
        Task ReportMessageAsync(string userId, ReportMessageDto dto);
    }
}
