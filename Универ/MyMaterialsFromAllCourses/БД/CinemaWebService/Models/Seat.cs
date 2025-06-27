using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace CinemaWebService.Models
{
    public class Seat
    {
        [BsonId] // Указывает, что это поле является идентификатором документа
        [BsonRepresentation(BsonType.ObjectId)] // Позволяет использовать строковое представление ObjectId
        public string? Id { get; set; } // Свойство для хранения _id из MongoDB опционально для него, для posgres оно должно неопциональным
        public int SeatId { get; set; } // PK
        public string Row { get; set; }
        public bool IsElectronic { get; set; }
        public int Duration { get; set; } = 0;

        // FK
        public int HallId { get; set; }
    }
}