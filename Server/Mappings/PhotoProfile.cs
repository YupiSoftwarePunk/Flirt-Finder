using AutoMapper;
using Server.Models;
using Server.DTOs;

namespace Server.Mappings
{
    public class PhotoProfile : Profile
    {
        public PhotoProfile()
        {
            CreateMap<Photo, PhotoDto>();
        }
    }
}
