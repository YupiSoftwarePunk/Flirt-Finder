using AutoMapper;
using Server.Models;
using Server.DTOs;

namespace Server.Mappings
{
    public class UserProfile : Profile
    {
        public UserProfile()
        {
            CreateMap<User, UserDto>();
            CreateMap<CreateUserDto, User>();
            CreateMap<UpdateUserDto, User>();
            CreateMap<PatchUserDto, User>();
            CreateMap<RegisterDto, User>();
            CreateMap<User, AuthResultDto>()
                .ForMember(dest => dest.Success, opt => opt.Ignore())
                .ForMember(dest => dest.Message, opt => opt.Ignore());
        }
    }
}
