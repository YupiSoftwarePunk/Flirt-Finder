using AutoMapper;
using Server.Models;
using Server.DTOs;

namespace Server.Mappings
{
    public class MessageProfile : Profile
    {
        public MessageProfile()
        {
            CreateMap<Message, MessageDto>()
                .ForMember(dest => dest.Content, opt => opt.MapFrom(src => src.Content))
                .ForMember(dest => dest.ReceiverId, opt => opt.MapFrom(src => src.ReceiverId));

            CreateMap<EditMessageDto, Message>()
                .ForMember(dest => dest.Content, opt => opt.MapFrom(src => src.NewContent));

            CreateMap<MessageDto, Message>()
                .ForMember(dest => dest.Content, opt => opt.MapFrom(src => src.Content))
                .ForMember(dest => dest.ReceiverId, opt => opt.MapFrom(src => src.ReceiverId));

            CreateMap<Message, PinMessageDto>()
                .ForMember(dest => dest.MessageId, opt => opt.MapFrom(src => src.Id));

        }
    }
}
