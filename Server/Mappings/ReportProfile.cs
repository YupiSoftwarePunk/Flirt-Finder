using AutoMapper;
using Server.Models;
using Server.DTOs;

namespace Server.Mappings
{
    public class ReportProfile : Profile
    {
        public ReportProfile()
        {
            CreateMap<ReportMessageDto, MessageReport>()
                .ForMember(dest => dest.ReportedAt, opt => opt.MapFrom(src => DateTime.UtcNow));
        }
    }
}
